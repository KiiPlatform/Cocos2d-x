/*
 * Copyright 2013 Kii
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *     
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.cocos2dx.cpp;

public interface Field {
    public static final String NAME = "name";
    public static final String TYPE = "type";
    public static final String AMOUNT = "amount";
    public static final String DISPLAYNAME = "display_name";

    public static final String _CREATED = "_created";

    public interface Type {
        public static final int INCOME = 1;
        public static final int EXPENSE = 2;
    }

    public static final String B_RANKING = "b_ranking02";
    public static final String SCORE = "score";

}
